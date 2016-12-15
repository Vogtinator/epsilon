#include "law_curve_view.h"
#include <assert.h>

namespace Probability {

LawCurveView::LawCurveView() :
  CurveView(),
  m_law(nullptr)
{
}

void LawCurveView::setLaw(Law * law) {
  m_law = law;
}

void LawCurveView::reload() {
  markRectAsDirty(bounds());
  computeLabels(Axis::Horizontal);
}

void LawCurveView::drawRect(KDContext * ctx, KDRect rect) const {
  ctx->fillRect(bounds(), KDColorWhite);
  drawAxes(Axis::Horizontal, ctx, rect);
  drawLabels(Axis::Horizontal, ctx, rect);
  if (m_law->isContinuous()) {
    drawCurve(m_law, KDColorRed, ctx, rect);
  } else {
    drawHistogram(m_law, KDColorRed, ctx, rect);
  }
}

float LawCurveView::min(Axis axis) const {
  assert(axis == Axis::Horizontal || axis == Axis::Vertical);
  return (axis == Axis::Horizontal ? m_law->xMin() : m_law->yMin());
}

float LawCurveView::max(Axis axis) const {
  assert(axis == Axis::Horizontal || axis == Axis::Vertical);
  return (axis == Axis::Horizontal ? m_law->xMax() : m_law->yMax());
}

float LawCurveView::gridUnit(Axis axis) const {
  assert(axis == Axis::Horizontal);
  return m_law->gridUnit();
}

char * LawCurveView::label(Axis axis, int index) const {
  assert(axis == Axis::Horizontal);
  return (char *)m_labels[index];
}

float LawCurveView::evaluateCurveAtAbscissa(void * law, float abscissa) const {
  return m_law->evaluateAtAbscissa(abscissa);
}

}
