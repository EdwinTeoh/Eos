#pragma once

#include <vector>
#include <algorithm>

#include <imgui_internal.h>
#include <ImGuizmo/ImSequencer.h>
#include <ImGuizmo/ImCurveEdit.h>

static const char* SequencerItemTypeNames[] =
{
	"Camera",
	"Music",
	"ScreenEffect",
	"FadeIn",
	"Animation"
};

struct RampEdit : public ImCurveEdit::Delegate
{
	ImVec2 mPts[3][8];
	size_t mPointCount[3];
	bool mbVisible[3];
	ImVec2 mMin;
	ImVec2 mMax;

	RampEdit()
	{
		mPts[0][0] = ImVec2(-10.0f, 0.0f);
		mPts[0][1] = ImVec2(20.f, 0.6f);
		mPts[0][2] = ImVec2(25.f, 0.2f);
		mPts[0][3] = ImVec2(70.f, 0.4f);
		mPts[0][4] = ImVec2(120.f, 1.f);
		mPointCount[0] = 5;

		mPts[1][0] = ImVec2(-50.f, 0.2f);
		mPts[1][1] = ImVec2(33.f, 0.7f);
		mPts[1][2] = ImVec2(80.f, 0.2f);
		mPts[1][3] = ImVec2(82.f, 0.8f);
		mPointCount[1] = 4;


		mPts[2][0] = ImVec2(40.f, 0);
		mPts[2][1] = ImVec2(60.f, 0.1f);
		mPts[2][2] = ImVec2(90.f, 0.82f);
		mPts[2][3] = ImVec2(150.f, 0.24f);
		mPts[2][4] = ImVec2(200.f, 0.34f);
		mPts[2][5] = ImVec2(250.f, 0.12f);
		mPointCount[2] = 6;
		mbVisible[0] = mbVisible[1] = mbVisible[2] = true;
		mMax = ImVec2(1.f, 1.f);
		mMin = ImVec2(0.f, 0.f);
	}

	size_t GetCurveCount()
	{
		return 3;
	}

	bool IsVisible(size_t curveIndex)
	{
		return mbVisible[curveIndex];
	}

	size_t GetPointCount(size_t curveIndex)
	{
		return mPointCount[curveIndex];
	}

	uint32_t GetCurveColor(size_t curveIndex)
	{
		uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
		return cols[curveIndex];
	}

	ImVec2* GetPoints(size_t curveIndex)
	{
		return mPts[curveIndex];
	}

	virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const 
	{ 
		return ImCurveEdit::CurveSmooth; 
	}

	virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
	{
		mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
		SortValues(curveIndex);
		for (size_t i = 0; i < GetPointCount(curveIndex); i++)
		{
			if (mPts[curveIndex][i].x == value.x)
				return (int)i;
		}
		return pointIndex;
	}

	virtual void AddPoint(size_t curveIndex, ImVec2 value)
	{
		if (mPointCount[curveIndex] >= 8)
			return;
		mPts[curveIndex][mPointCount[curveIndex]++] = value;
		SortValues(curveIndex);
	}

	virtual ImVec2& GetMax() { return mMax; }
	virtual ImVec2& GetMin() { return mMin; }
	virtual unsigned int GetBackgroundColor() { return 0; }

private:
	void SortValues(size_t curveIndex)
	{
		auto b = std::begin(mPts[curveIndex]);
		auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
		std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });
	}
};

// Interface with sequencer
struct Sequence : public ImSequencer::SequenceInterface
{
	// Data
	int mFrameMin, mFrameMax;
	struct SequenceItem
	{
		int mType;
		int mFrameStart, mFrameEnd;
		bool mExpanded;
	};
	std::vector<SequenceItem> Items;
	RampEdit rampEdit;

	Sequence() : mFrameMin{ 0 }, mFrameMax{ 0 } {}

	// Functions
	virtual int GetFrameMin() const { return mFrameMin; }
	virtual int GetFrameMax() const { return mFrameMax; }
	virtual int GetItemCount() const { return (int)Items.size(); }

	virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
	virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
	virtual const char* GetItemLabel(int index) const
	{
		static char tmps[512];
		sprintf_s(tmps, "[%02d] %s", index, SequencerItemTypeNames[Items[index].mType]);
		return tmps;
	}

	virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
	{
		SequenceItem& item = Items[index];
		if (color)
			*color = 0xFFAA8080; // same color for everyone, return color based on type
		if (start)
			*start = &item.mFrameStart;
		if (end)
			*end = &item.mFrameEnd;
		if (type)
			*type = item.mType;
	}

	virtual void Add(int type) { Items.push_back(SequenceItem{ type, 0, 10 }); };
	virtual void Del(int index) { Items.erase(Items.begin() + index); }
	virtual void Duplicate(int index) { Items.push_back(Items[index]); }

	virtual size_t GetCustomHeight(int index)
	{
		return Items[index].mExpanded ? 300 : 0;
	}

	virtual void DoubleClick(int index)
	{
		if (Items[index].mExpanded)
		{
			Items[index].mExpanded = false;
			return;
		}
		for (auto& item : Items)
			item.mExpanded = false;
		Items[index].mExpanded = !Items[index].mExpanded;
	}

	/*virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
	{
		static const char* labels[] = { "Translation", "Rotation", "Scale" };

		rampEdit.mMax = ImVec2(float(mFrameMax), 1.0f);
		rampEdit.mMin = ImVec2(float(mFrameMin), 0.0f);
		draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
		for (int i = 0; i < 3; i++)
		{
			ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
			ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
			draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
			if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
				rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
		}
		draw_list->PopClipRect();

		ImGui::SetCursorScreenPos(rc.Min);
		float xdiff = rc.Max.x - rc.Min.x;
		float ydiff = rc.Max.y - rc.Min.y;
		ImVec2 diff{ xdiff, ydiff };
		ImCurveEdit::Edit(rampEdit, diff, 137 + index, &clippingRect);
	}*/

	virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
	{
		rampEdit.mMax = ImVec2(float(mFrameMax), 1.0f);
		rampEdit.mMin = ImVec2(float(mFrameMin), 0.0f);
		draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < rampEdit.mPointCount[i]; j++)
			{
				float p = rampEdit.mPts[i][j].x;
				if (p < Items[index].mFrameStart || p > Items[index].mFrameEnd)
					continue;
				float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
				float x = ImLerp(rc.Min.x, rc.Max.x, r);
				draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
			}
		}
		draw_list->PopClipRect();
	}
};

